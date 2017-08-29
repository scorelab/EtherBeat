/**
 * Module dependencies.
 */
import * as express from "express";
import * as compression from "compression";  // compresses requests
import * as session from "express-session";
import * as bodyParser from "body-parser";
import * as logger from "morgan";
import * as errorHandler from "errorhandler";
import * as lusca from "lusca";
import * as dotenv from "dotenv";
import * as flash from "express-flash";
import * as path from "path";
import * as graphqlHTTP from "express-graphql";
import * as passport from "passport";

import expressValidator = require("express-validator");


/**
 * Load environment variables from .env file, where API keys and passwords are configured.
 */
dotenv.config({ path: ".env.example" });

/**
 * API keys and Passport configuration.
 */
import * as passportConfig from "./config/passport";

/**
 * Controllers (route handlers).
 */
import * as homeController from "./controllers/home";
import * as apiController from "./controllers/api";

/**
 * Models
 */
import * as models from "./models/schema";

/**
 * GraphQL resolvers
 */
import * as resolvers from "./resolvers/root";

/**
 * Create Express server.
 */
const app = express();


/**
 * Express configuration.
 */
app.set("port", process.env.PORT || 3000);
app.use(compression());
app.use(logger("dev"));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(expressValidator());
app.use(session({
  resave: true,
  saveUninitialized: true,
  secret: process.env.SESSION_SECRET
}));

app.use(passport.initialize());
app.use(passport.session());
app.use(flash());
app.use(lusca.xframe("SAMEORIGIN"));
app.use(lusca.xssProtection(true));
app.use((req, res, next) => {
  res.locals.user = req.user;
  next();
});
app.use((req, res, next) => {
  // After successful login, redirect back to the intended page
  if (!req.user &&
    req.path !== "/login" &&
    !req.path.match(/^\/auth/) &&
    !req.path.match(/\./)) {
    req.session.returnTo = req.path;
  }
  next();
});

/**
 * Primary app routes.
 */
app.get("/", homeController.index);

/**
 * API examples routes.
 */
app.get("/api", passportConfig.isAuthenticated, apiController.getApi);

/**
 * OAuth authentication routes. (Sign in)
 */
app.get("/auth/github",
  passport.authenticate("github", { scope: ["user:email"] }),
  (req, res) => {
    // The request will be redirected to GitHub for authentication
  });

// GET /auth/github/callback
app.get("/auth/github/callback",
  passport.authenticate("github", { failureRedirect: "/login" }),
  (req, res) => {
    res.redirect("/graphql");
  });

app.get("/login", function(req, res){
  res.redirect("/auth/github");
});

app.get("/logout", (req, res) => {
  req.logout();
  res.redirect("/");
});

/**
 * GraphQL endpoint
 */
app.get("/graphql", passportConfig.isAuthenticated, graphqlHTTP({
  schema: models.schema,
  rootValue: resolvers.root,
  graphiql: true,
}));

app.post("/graphql", passportConfig.isAuthenticated, graphqlHTTP({
  schema: models.schema,
    rootValue: resolvers.root,
  graphiql: false
}));

/**
 * Error Handler. Provides full stack - remove for production
 */
app.use(errorHandler());

/**
 * Start Express server.
 */
app.listen(app.get("port"), () => {
  console.log(("  App is running at http://localhost:%d in %s mode"), app.get("port"), app.get("env"));
  console.log("  Press CTRL-C to stop\n");
});

module.exports = app;
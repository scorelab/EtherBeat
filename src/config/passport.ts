import * as passport from "passport";
import * as passportGithub from "passport-github2";
import * as passportLocal from "passport-local";
import * as bcrypt from "bcryptjs";
import * as _ from "lodash";

// import { default as User } from "../models/User";
import { Request, Response, NextFunction } from "express";

const GitHubStrategy = passportGithub.Strategy;
const LocalStrategy  = passportLocal.Strategy;

passport.serializeUser<any, any>((user, done) => {
  done(undefined, user.id);
});

passport.deserializeUser((id, done) => {
  done(undefined, id);
});

/**
 * OAuth Strategy Overview
 *
 * - User is already logged in.
 *   - Check if there is an existing account with a provider id.
 *     - If there is, return an error message. (Account merging not supported)
 *     - Else link new OAuth account with currently logged-in user.
 * - User is not logged in.
 *   - Check if it's a returning user.
 *     - If returning user, sign in and we are done.
 *     - Else check if there is an existing account with user's email.
 *       - If there is, return an error message.
 *       - Else create a new account.
 */

/**
 * GitHub strategy
 */
passport.use(new GitHubStrategy({
  clientID: process.env.GITHUB_CLIENT_ID,
  clientSecret: process.env.GITHUB_CLIENT_SECRET,
  callbackURL: "http://localhost:3000/auth/github/callback"
},
  (accessToken: any, refreshToken: any, profile: any, done: any) => {
    return done(undefined, profile);
  }
));

passport.use(new LocalStrategy({ 
  usernameField: 'email'
},
  (email, password, done)=> {
    User.findOne({ email },(err, user:any)=>{
      if (err) { return done(err); }
      if (!user) {
        return done(null, false, { message: 'Email not registered' });
      }
      bcrypt.compare(password, user.password,(err:Error, result:boolean)=>{
        if(err) return done(err);
        if(result) return done(null, user);
        return done(null, false, { message: 'Incorrect password.' });
      });
    }); 
  }
));

/**
 * Login Required middleware.
 */
export let isAuthenticated = (req: Request, res: Response, next: NextFunction) => {
  if (req.isAuthenticated()) {
    return next();
  }
  res.redirect("/login");
};

/**
 * Authorization Required middleware.
 */
export let isAuthorized = (req: Request, res: Response, next: NextFunction) => {
  const provider = req.path.split("/").slice(-1)[0];

  if (_.find(req.user.tokens, { kind: provider })) {
    next();
  } else {
    res.redirect(`/auth/${provider}`);
  }
};

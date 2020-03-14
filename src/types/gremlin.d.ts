import { EventEmitter } from 'events';
import { Readable } from 'stream';

export interface Bindings {
  [key: string]: any;
}

export interface QueryObject {
  gremlin: string;
  bindings: Bindings;
}

export class WebSocketGremlinConnection extends EventEmitter {
  open: boolean;
  sendMessage (message: string): void;
  handleMessage (message: string): void;
}

export class MessageStream extends Readable {}

export class GremlinClient extends EventEmitter {
  port: number;
  host: string;
  options: ClientOptions;
  useSession: boolean;
  sessionId: string;
  connected: boolean;
  connection: WebSocketGremlinConnection;

  execute <T> (script: QueryObject, callback: (err: Error | null, results: T[]) => void): void;
  execute <T> (script: string, callback: (err: Error | null, results: T[]) => void): void;
  execute <T> (script: string, bindings: Bindings, callback: (err: Error | null, results: T[]) => void): void;
  execute <T> (script: string, bindings: Bindings, message: any, callback: (err: Error | null, results: T[]) => void): void;

  messageStream (script: QueryObject): MessageStream;
  messageStream (script: string): MessageStream;
  messageStream (script: string, bindings: Bindings): MessageStream;
  messageStream (script: string, bindings: Bindings, message: any): MessageStream;

  readable (script: QueryObject): Readable;
  readable (script: string): Readable;
  readable (script: string, bindings: Bindings): Readable;
  readable (script: string, bindings: Bindings, message: any): Readable;
}

export interface ClientOptions {
  /**
   * Whether to use sessions or not (default: `false`).
   */
  session?: boolean;
  /**
   * The script engine to use on the server, see your gremlin-server.yaml file (default: `"gremlin-groovy"`).
   */
  language?: string;
  /**
   * The name of the "operation" to execute based on the available OpProcessor (default: `"eval"`).
   */
  op?: string;
  /**
   * The name of the OpProcessor to utilize (default: `""`).
   */
  processor?: string;
  /**
   * Mime type of returned responses, depending on the serializer (default: `"application/json"`).
   */
  accept?: string;
  /**
   * A custom URL connection path if connecting to a Gremlin server behind a WebSocket proxy.
   */
  path?: string;
}

export function createClient (port: number, host: string, options?: ClientOptions): GremlinClient;
export function createClient (port: number, options?: ClientOptions): GremlinClient;
export function createClient (options?: ClientOptions): GremlinClient;

export interface QueryPromise <T> extends Promise<T> {
  readonly query: string;
}

export interface TemplateTag {
  <T> (gremlinChunks: TemplateStringsArray, ...values: any[]): QueryPromise<T[]>;
}

export function makeTemplateTag (client: GremlinClient): TemplateTag;

export interface Functions {
  [key: string]: (...args: any[]) => QueryObject;
}

export interface BoundFunctions {
  [key: string]: <T> (...args: any[]) => QueryPromise<T[]>;
}

export function bindForClient (client: GremlinClient, functions: Functions): BoundFunctions;

declare let _default:  { 
  createClient, 
  makeTemplateTag, 
  bindForClient 
};

export default _default;

/*
 * NodePlugin
 * Copyright (c) 2013 Matt <matthieu@gonnet.tv>
 */

/*
 * Module dependencies.
 */

/*
 * Export `Canvas` as the module.
 */

 var gif = require('./bindings')
,    NodeGif = gif.NodeGif;

var NodeGif = exports = module.exports = NodeGif;

/*
 * Library version.
 */

exports.version = '0.0.1';


/*
 * Expose constructors.
 */

 exports.NodeGif = NodeGif;
//

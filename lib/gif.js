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
,    GifWrapper = gif.GifWrapper;

var GifWrapper = exports = module.exports = GifWrapper;

/*
 * Library version.
 */

exports.version = '0.0.1';


/*
 * Expose constructors.
 */

 exports.GifWrapper = GifWrapper;
//

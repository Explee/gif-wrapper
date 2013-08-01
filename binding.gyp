{
  'targets': [
    {
      'target_name': 'gif',
      'sources': [ 'src/node-gif.cc',
                    'src/init.cc',
                    'headers/node-gif.h'
                 ],
  'defines': [
          'DEBUG_MODE'
  ],
  'cflags': [ '-fexceptions', '-Wall', '-g3'],
  'cflags_cc!': [ '-fno-exceptions' ],
      'conditions': [
        ['OS=="win"', {
          'libraries': [
          ],
          'include_dirs': [
          '/usr/local/include/opencv2/',
            '<(GTK_Root)/include',
          ],
          'defines': [
            'snprintf=_snprintf',
            '_USE_MATH_DEFINES'  # for M_PI
          ]
        }, {  # 'OS!="win"'
          'libraries': [
          '-lgif',
	     '-lopencv_core',
              '-lopencv_imgproc',
              '-lopencv_calib3d',
              '-lopencv_features2d',
              '-lopencv_objdetect',
              '-lopencv_video',
              '-lopencv_highgui',
              '-lopencv_contrib',
              '-lopencv_flann',
              '-lopencv_ml',
              '-lopencv_legacy'
           ]
        }],
        [ 'OS!="Win"',
          {
            'include_dirs':[],
              'xcode_settings': {
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
            },
            'libraries':[
       ]
          }
          ],
      ]
    }
  ]
}

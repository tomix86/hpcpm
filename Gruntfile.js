module.exports = function(grunt) {

  require('load-grunt-tasks')(grunt);
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),

    karma: {
      unit: {
        configFile: 'karma.conf.js'
      }
    },

    express: {
      dev: {
        options: {
          script: 'server.js'
        }
      },
      prod: {
        options: {
          script: 'dist/server.js'
        }
      }
    },

    uglify: {
      options: {
        mangle: true,
        compress: true
      },
    },

    useminPrepare: {
      html: 'public/index.html',
      options: {
        dest: 'dist'
      }
    },

    usemin: {
	  	html:['dist/public/index.html']
  	},

    copy: {
	    html: {
	    	src: 'public/index.html', dest: 'dist/public/index.html'
	    },
      fonts: {
	src: 'public/fonts/*', dest: 'dist/'
      },
      favicon: {
        src: 'public/favicon.ico', dest: 'dist/public/favicon.ico'
      },
      bower: {
        src: 'public/js/bower.js', dest: 'dist/public/js/bower.js'
      },
      server: {
        src: 'server.js', dest: 'dist/server.js'
      },
      config: {
        src: 'config.js', dest: 'dist/config.js'
      },
      package: {
        src: 'package.json', dest: 'dist/package.json'
      }
	  },

    watch: {
      dev: {
        files: ['<%= jshint.files %>'],
        tasks: ['jshint', 'karma:unit']
      },
      prod: {
        options: { livereload: false},
        files: ['<%= jshint.files %>'],
        tasks: ['build']
      }
    },

    jshint: {
      files: ['Gruntfile.js', 'server.js', 'config.js', 'public/js/*.js', 'test/*.js'],
      options: {
        globals: {
          jQuery: true,
          console: true,
          module: true,
          document: true
        }
      }
    },
  });

  grunt.registerTask('lint', ['jshint']);
  grunt.registerTask('test', ['karma:unit']);
  grunt.registerTask('run', ['express:dev', 'watch:dev']);
  grunt.registerTask('build', [
    'jshint',
    'copy',
    'useminPrepare',
    'concat:generated',
    'cssmin:generated',
    'uglify:generated',
    'usemin'
  ]);
  grunt.registerTask('serve', ['build', 'express:prod', 'watch:prod']);
};

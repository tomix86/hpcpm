module.exports = function(grunt) {

  require('load-grunt-tasks')(grunt);
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),

    nodemon: {
      dev: {
        script: 'server.js'
      },
      dist: {
        script: 'dist/server.js'
      }
    },

    bower_concat: {
      options : {
        sourceMap :true
      },
      all: {
        dest: 'public/js/bower.js'
      }
    },

    uglify: {
      bower: {
        options: {
          mangle: true,
          compress: true
        },
        files: {
          'dist/public/js/bower.min.js': 'dist/public/js/bower.js'
        }
      }
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
      bower: {
        src: 'public/js/bower.js', dest: 'dist/public/js/bower.js'
      },
      server: {
        src: 'server.js', dest: 'dist/server.js'
      },
      config: {
        src: 'config.js', dest: 'dist/config.js'
      }
	  },

    watch: {
      files: ['Gruntfile.js', 'server.js', 'config.js', 'public/js/*.js', 'test/*.js'],
      tasks: ['jshint', 'nodemon']
    },

    jshint: {
      files: ['<%= watch.files %>'],
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

  grunt.registerTask('test', ['jshint']);
  grunt.registerTask('run-dev', ['jshint', 'nodemon:dev']);
  grunt.registerTask('run', ['nodemon:dist']);
  grunt.registerTask('build', [
    'jshint',
    'copy:server',
    'copy:config',
    'copy:html',
    'useminPrepare',
    'concat:generated',
    'cssmin:generated',
    'uglify:generated',
    'usemin'
  ]);
  grunt.registerTask('all', ['build', 'run']);
};

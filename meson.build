project('ParallelBFS', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

incdir = include_directories('include')

executable('bfs',
           'src/main.c',
           'src/coo.c',
           install : true,
           include_directories : incdir)

project('ParallelBFS', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

incdir = include_directories('include')
subdir('src')

executable('bfs',
           bfs_src,
           install : true,
           include_directories : incdir)

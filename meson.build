project('ParallelBFS', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

openmp_dep = dependency('openmp')
incdir = include_directories('include')
subdir('src')

executable('bfs',
           bfs_src,
           install : true,
           include_directories : incdir,
           dependencies : openmp_dep)

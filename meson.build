project('ParallelBFS', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

cc = meson.get_compiler('c')
math_dep = cc.find_library('m')
openmp_dep = dependency('openmp')
incdir = include_directories('include')
subdir('src')

bfs = executable('bfs',
           bfs_src,
           install : true,
           include_directories : incdir,
           dependencies : [openmp_dep, math_dep])

benchmark('bench_bfs', bfs, args : ['../bench/web-Stanford.in'])

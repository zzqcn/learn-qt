project('HelloWorld', 'cpp', default_options : ['cpp_std=c++17'])

# Documentation: https://mesonbuild.com/Qt5-module.html
# https://stackoverflow.com/questions/56443458/how-to-specify-rpath-for-debug-builds-in-meson
qt6 = import('qt6')
qt6dep = dependency('qt6', modules: ['Core', 'Widgets'])

generated_files = qt6.preprocess(
    moc_headers : 'MainWindow.h',
    ui_files : 'MainWindow.ui',
    dependencies: [qt6dep])

executable('HelloWorld',
    'main.cpp',
    'MainWindow.cpp',
    generated_files,
    dependencies : [qt6dep],
    build_rpath : '/usr/local/Qt-6.2.4/lib',
    install : true
)

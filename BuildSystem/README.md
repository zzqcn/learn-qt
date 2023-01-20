# Qt编译系统

全文见: yuque.com/zzqcn/XXX

## Qt Creator + qmake

很早就支持, 没有问题.

## Qt Creator + CMake

最近才支持, 没有问题. 以后将替换 qmake?

## Qt Creator + meson

有限的支持. 在 macOS 上编译 Qt6 有问题, Qt5 没试过. 问题会导致在 Qt Creator 里不能直接点界面上的调试或运行来启动程序. 问题有:
- Qt Creator 的 meson 插件没有在编译目录生成 build.ninja, 导致后续编译步骤(ninja all)失败
- 自动生成的 meson.build 有问题, 明明是 Qt6 还在找 Qt5
- 生成的程序没有处理好动态库依赖关系, 导致手动运行也报错

暂时的解决办法:
- 手动修改 meson.build, 可对比 HelloWorld-meson/ 下的 meson.build 与 meson.build.old, 后者是自动生成的有问题的文件
- 我用的 Qt6 是自己从源码编译的, 安装在特定目录, 要在 meson.build 的 target 中添加 build_rpath 指定库查找路径, 如
  ```makefile
  executable('HelloWorld',
      'main.cpp',
      'MainWindow.cpp',
      generated_files,
      dependencies : [qt6dep],
      build_rpath : '/usr/local/Qt-6.2.4/lib',
      install : true
  )
  ```
- 不要在 Qt Creator 上点击编译或直接运行, 手动编译运行
  ```bash
  $meson setup <builddir>
  $ninja -C <builddir>
  ```
- 如果需要加断点调试等, 可以在 Visual Studio Code 中配置生成和调试环境

## Visual Studio + Qt扩展 + Qt Creator

在 Windows 上, Qt 提供了 Visual Studio 扩展, 从而使得除了用 Qt Creator 等打开 .ui 文件编辑界面外, 其他工作都可以在 Visual Studio IDE 中无缝完成.

// TODO

## 参考

- https://doc.qt.io/qtcreator/creator-project-other.html
- https://doc.qt.io/qtvstools/index.html
- https://stackoverflow.com/questions/56443458/how-to-specify-rpath-for-debug-builds-in-meson

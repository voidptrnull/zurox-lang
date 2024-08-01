# Basic Usage

>Assumption: You have the binaries and environment variables properly set. This will also assume that you have your binary has 'zuroxc'.

Most of the time, you will want to create an executable directly from the file. If your file is `main.zx` for instance, you will have to invoke the following command:
```sh
zuroxc main.zx
```

Note that `zuroxc` is not meant to be used by itself as it is unable to handle dependencies. This basically excludes anything except the `zxstd`. For such projects, refer to the Zurox Package Manager (ZXPM) instead.
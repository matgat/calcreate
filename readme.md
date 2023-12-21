# [calcreate](https://github.com/matgat/calcreate.git)

Written to check out `std::chrono` facilities, creates calendar log entries

## Usage
Windows binary is dynamically linked to Microsoft c++ runtime,
so needs the installation of
[`VC_redist.x64.exe`](https://aka.ms/vs/17/release/vc_redist.x64.exe)
as prerequisite.

To see the available command line arguments:
```
$ .\calcreate
```

Create calendar log entries:
```
$ .\calcreate -start 2022-05-16 -weeks 30
```
Then paste the clipboard content


## Build
```
$ git clone https://github.com/matgat/calcreate.git
> msbuild msvc/calcreate.vcxproj -t:Rebuild -p:Configuration=Release -p:Platform=x64
```

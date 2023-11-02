@echo off
REM Visual Studio Developer Command Promptを使用する場合
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

cd "DirectXGame"
REM 変更をすべて取り消す
git reset --hard
REM Gitで最新のコードを取得
git pull origin LWP_20231031

REM MSBuildのパス
set MSBUILD_PATH="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
VSINSTALLDIR=MSBuild\Microsoft\VC\<version>\

REM Visual Studioでビルド
%MSBUILD_PATH% "DirectXGame.sln"
pause
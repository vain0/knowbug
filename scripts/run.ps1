#!/bin/pwsh
# HSP3 スクリプトを実行する

# 使い方
#   ./scripts/run <script> <runtime-name>

$hspRoot = $env:HSP3_ROOT
$workDir = (get-item .).fullName

$scriptName = (get-item $args[0]).fullName
$runtimeName = $args[1]

$axName = [system.io.path]::changeExtension($scriptName, ".ax")
$baseDir = [system.io.path]::getDirectoryName((get-item $scriptName).fullName)

if (!$hspRoot) {
    write-error "環境変数 HSP3_ROOT を設定してください。"
    exit 1
}

if (!$scriptName) {
    write-error "コンパイルするファイル名を引数に指定してください。"
    exit 1
}

if ($mode = "hsp3") {
    $compilerArgs = @("-d", "-w")
} elseif ($mode = "hsp3utf") {
    $compilerArgs = @("-d", "-w", "-i", "-u")
} else {
    write-error "ランタイムが不明です。hsp3 か hsp3utf を指定してください。"
    exit 1
}

try {
    cd $baseDir
    & "$hspRoot/hspcmp" "--compath=$hspRoot/common/" $compilerArgs $scriptName
    if (!$?) {
        exit 1
    }

    & "$hspRoot/$runtimeName" $axName
} finally {
    cd $workDir
}

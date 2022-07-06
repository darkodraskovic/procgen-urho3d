PROJ_DIR=c:/Users/darko/Development/procgen-urho3d
CONFIGURE=false
CONFIGURATION=Release
TARGET=ProcGen.vcxproj
export CB_TEST_DATA="c:/Users/darko/Development/cloudberrie-processor-cpp/Tests/Data"

while getopts "t:cd" option; do
    case $option in
        c)
            CONFIGURE=true
            ;;
        d)
            CONFIGURATION=Debug
            ;;
        t)
            TARGET=$OPTARG
            ;;
    esac
done

validate_exit() {
    if [ $? != 0 ]; then
        exit 1
    fi
}

if [ $CONFIGURE = true ]; then
    echo "INFO: configure MSBuild"
    cd ${PROJ_DIR}/build
    cmake ..
    validate_exit

    echo "INFO: configure Ninja"    
    cd $PROJ_DIR/ninja/
    cmake -G"Ninja" ..
    mv compile_commands.json ..
    validate_exit
fi

cd $PROJ_DIR/build

if [ "$TARGET" == "ProcGen.sln" ]; then
    MSBuild.exe -target:"Build" /property:Configuration=${CONFIGURATION} .\\${TARGET}
    validate_exit
    exit 0
fi

MSBuild.exe -target:"Build" /property:Configuration=${CONFIGURATION} .\\${TARGET}
validate_exit

cd ${PROJ_DIR}/build/bin
./ProcGen.exe

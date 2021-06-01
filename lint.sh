#!/bin/bash

# exit when any command fails
set -e

display_usage() { 
	echo "Lint checking script for both cpp and node codes."
    echo -e "If no argument is passed, it runs with check mode and --all option as default.\n"
	echo "Usage:"
    echo -e "$0 [check | fix] [--all | --cpp | --node]"
    echo -e "$0 -h | --help" 
    echo ""
}

# Tips: clang-format without this script
# Linux + fix lint
# $ find . -regex '^\./\(src\|tests\|benchmark\)/.*\.\(cpp\|h\)$' -not -path './src/jsui/*' | xargs clang-format -i

# Linux + check lint
# $ find . -regex '^\./\(src\|tests\|benchmark\)/.*\.\(cpp\|h\)$' -not -path './src/jsui/*' | xargs clang-format -i --dry-run --Werror

# Mac + fix lint
# $ find -E . -regex '^\./(src|tests|benchmark)/.*\.(cpp|h)$' -not -path './src/jsui/*' | xargs clang-format -i

# Mac + check lint
# $ find -E . -regex '^\./(src|tests|benchmark)/.*\.(cpp|h)$' -not -path './src/jsui/*' | xargs clang-format -i --dry-run 

TARGET="all"
MODE="check"

for i in "$@"
do
    if [ "$i" == "fix" ]
    then
        MODE="fix"
    fi

    if [ "$i" == "--cpp" ]
    then
        TARGET="cpp"
    fi

    if [ "$i" == "--node" ]
    then
        TARGET="node"
    fi

    if [ "$i" == "--help" ] || [ "$i" == "-h" ]
    then
        display_usage
        exit 0
    fi
done

OS="Unknown"
if [ "$(uname)" == "Darwin" ]; then
    # Do something under Mac OS X platform
    OS="MacOSX"
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    # Do something under GNU/Linux platform
    OS="Linux"
elif [ "$(expr substr $(uname -s) 1 5)" == "MINGW" ]; then
    # Do something under Windows platform
    OS="Windows"
fi


if [ "$TARGET" == "all" ] || [ "$TARGET" == "node" ]; then
    echo "[$0] $MODE node files"
    if [ "$MODE" == "fix" ]; then
        npm --prefix=src/jsui run lint:fix
    else
        npm --prefix=src/jsui run lint
    fi
    echo "[$0] OK"
fi

if [ "$OS" == "MacOSX" ]; then
    CPP_FILES=`find -E . -regex '^\./(src|tests|benchmark)/.*\.(cpp|h)$' -not -path './src/jsui/*'`
elif [ "$OS" == "Linux" ] || [ "$OS" == "Windows" ]; then
    CPP_FILES=`find . -regex '^\./\(src\|tests\|benchmark\)/.*\.\(cpp\|h\)$' -not -path './src/jsui/*'`
else
    echo "Unknow OS"
    exit 1
fi


if [ "$TARGET" == "all" ] || [ "$TARGET" == "cpp" ]; then
    echo "[$0] $MODE cpp files"   
    if [ "$MODE" == "fix" ]; then
        clang-format -i $CPP_FILES
    else
        clang-format -i --dry-run --Werror $CPP_FILES
    fi
    echo "[$0] OK"
fi

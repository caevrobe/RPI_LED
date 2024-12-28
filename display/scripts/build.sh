#!/usr/bin/env bash

# load environment cfgs from build.sh directory
dir=$(dirname $0)
cd $dir
. "environment.sh"
cd ..

# prints usage message
usage() {
   echo -e "Usage: $0 [--env=environment]\n\nOptions"
   printf '\t%-10s the environment to build for (defined in environment.sh)\n' '--env'
   exit $1
}

# prints fatal error and exits
fatal() {
   echo -e "\033[0;31m$1\033[0m"
   
   if [[ $2 -eq "usage" ]]; then
      echo -e "\n"
      usage 1
   fi
   
   exit 1
}

success() {
   echo -e "\033[0;32m$1\033[0m"
}

# parse script arguments
while getopts :-: opt; do
    opt="${OPTARG%=*}"
    OPTARG="${OPTARG#*=}"

    case "${opt:0:1}" in
      e)
         export ENVIRONMENT=$OPTARG;;
      h|*)
         usage;;
   esac
done

# check for missing arguments
[ -z $ENVIRONMENT ] && fatal "Must specify build environment." "usage"

# check if specified environment config exists and has required keys
declare -n cfg="cfg_"$ENVIRONMENT
! [[ -v cfg[${required_keys[0]}] ]] && fatal "No configuration found for $ENVIRONMENT environment."
for key in ${required_keys[@]}; do
    [[ -z ${cfg[$key]} ]] && fatal "$ENVIRONMENT configuration object missing key $key."
done

printf '\nLoaded environment config.'
printf '\n%-15s %s\n' 'Environment:' $ENVIRONMENT


# TODO build required objects string
# * a .o file for all .cc files in animation


# TODO build include strings

IFS=$'\n'
paths=($(find src/ -name "*.cc" ! -name '*main.cc' ! -path '*/canvas/*'))
unset IFS

# build required .o file string for main.cc
# from all .cc files in src/
OBJS=''
echo -e "\nFinding .cc files for main.cc target dependencies..."
for fpath in "${paths[@]}"; do
   current="${fpath%%.*}"
   echo "Found ${current}.cc"

   OBJS+="${current}.o "
done

# MAKECMD="make $ENVIRONMENT OBJS=\"src/canvas/canvas.o ${OBJS}\" CNV_SRC=\"${cfg["CNV_SRC"]}\""
# echo -e "\nBuilding with: ${MAKECMD}"
MAKECMD="make out/RPI_LED${cfg["OUT_EXT"]} OBJS=\"src/canvas/canvas.o lib/simdjson/simdjson.cpp ${OBJS}\" "
MAKECMD+="EXT=\"${cfg["OUT_EXT"]}\" "
MAKECMD+="CNV_SRC=\"${cfg["CNV_SRC"]}\" "
MAKECMD+="CNV_INC=\"${cfg["CNV_INC"]}\" "
MAKECMD+="CNV_LIB=\"${cfg["CNV_LIB"]}\" "
MAKECMD+="PREPROC=\"-DCANVAS=${cfg["CNV_VER"]}\" "
MAKECMD+="BST_INC=\"${cfg["BST_INC"]}\""

echo -e "\nBuilding with: ${MAKECMD}"
eval "${MAKECMD}"
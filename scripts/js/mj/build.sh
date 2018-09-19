#./build.sh hnmj 1.0.0
project=$1
ver=$2
rm -rf ../../../OutPut/${project}/scripts/
rm -rf ../../../OutPut/${project}/res/
cocos jscompile -s ./scripts/ -d ../../../OutPut/${project}/scripts
cp -r ./res/ ../../../OutPut/${project}/res/
cp -r ./scripts/proto/ ../../../OutPut/${project}/scripts/proto/
find ../../../OutPut/${project} -name "*svn*" | xargs rm -rf
cd ../../../
node version_generator.js -s OutPut -d OutPut/${project}/version/ -sEx ${project}/ -vEx ${project}/version/  -v ${ver}
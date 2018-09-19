curDir=$(dirname $0)
cd ${curDir}
project=public
ver=2.0.23
ver1=$1
if test -z "$ver1";then
	echo "no input version"
else
	ver=$ver1
fi
echo "version:"$ver
rm -rf ../OutPut/script/
rm -rf ../OutPut/${project}/res/
cocos jscompile -s ./script/ -d ../OutPut/script/
mkdir -p ../OutPut/${project}/res/
cp -r ./res/ ../OutPut/${project}/res/
find ../OutPut -name "*svn*" | xargs rm -rf
cd ../
node version_generator.js -s OutPut -d OutPut/${project}/version/ -sEx ${project}/,script/ -vEx ${project}/version/  -v ${ver}
cd ./OutPut/
rm -rf ${project}_${ver}.zip
zip -r ${project}_${ver}.zip ${project} script
echo "version:"$ver
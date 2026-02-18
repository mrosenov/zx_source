#!/bin/bash
#######################################################################
#	zgame zskill znet  Jade Dynasty Server side compilation
#######################################################################

LOGFILE="/root/zx_source/build.log"

# Redirect ALL output (stdout + stderr) to console AND logfile
exec > >(tee -a "$LOGFILE") 2>&1

BUILD_START_TS=$(date +%s)
echo ""
echo "=================================================================="
echo " BUILD STARTED: $(date '+%Y-%m-%d %H:%M:%S')"
echo "=================================================================="
echo ""

GS=`echo *game`;
NET=`echo *net`;
SKILL=`echo *skill`;
echo ""
echo "=========================== setup $NET ==========================="
echo ""
echo ""
echo "=========================== setup iozlib ==========================="
echo ""
if [ ! -d iozlib ]; then
	mkdir iozlib
fi;
cd iozlib;
if [ ! -d inc ]; then
	mkdir inc
fi;
cd inc
rm *
ln -s ../../$NET/gamed/auctionsyslib.h
ln -s ../../$NET/gdbclient/db_if.h
ln -s ../../$NET/gamed/factionlib.h
ln -s ../../$NET/common/glog.h
ln -s ../../$NET/gamed/gmattr.h
ln -s ../../$NET/gamed/gsp_if.h
ln -s ../../$NET/gamed/mailsyslib.h
ln -s ../../$NET/gamed/privilege.hxx
ln -s ../../$NET/gamed/sellpointlib.h
ln -s ../../$NET/gamed/stocklib.h

cd ..
rm lib*
ln -s ../$NET/io/libgsio.a
ln -s ../$NET/gdbclient/libdbCli.a
ln -s ../$NET/gamed/libgsPro2.a
ln -s ../$NET/logclient/liblogCli.a
cd ..
echo ""
echo "======================== modify Rules.make ========================"
echo ""
EPWD=`pwd|sed -e 's/\//\\\\\//g'`;
cd $GS
sed -i -e "s/IOPATH=.*$/IOPATH=$EPWD\/iozlib/g" -e "s/BASEPATH=.*$/BASEPATH=$EPWD\/$GS/g" Rules.make
echo ""
echo "====================== softlink libskill.so ======================="
echo ""
cd gs
rm libskill.so
ln -s ../../$SKILL/skill/libskill.so 
cd ../../

buildrpcgen()
{
echo ""
	echo "========================== $NET rpcgen ============================"
echo ""
	cd $NET
	./rpcgen
	cd ..

}
buildgslib() 
{

echo ""
	echo "======================= build liblogCli.a ========================="
echo ""
	cd $NET
	cd logclient
	make clean
	make -f Makefile.gs -j8
	cd ..
echo ""
	echo "======================== build libgsPro2.a ========================="
echo ""
	cd gamed
	make clean
	make lib -j8
	cd ..
echo ""
	echo "======================== build libdbCli.a =========================="
echo ""
	cd gdbclient
	make clean
	make lib -j8
	make dbtool -j8
	cd ..
	cd ..
echo ""
	echo "============================ make libgs ============================"
echo ""
	cd $GS
	cd libgs
	mkdir -p io
	mkdir -p gs
	mkdir -p db
	mkdir -p log
	mkdir -p dbtool
	make
	cd ../../
}
builddumpitem()
{
	buildrpcgen;
	buildgslib;
	cd $GS
	cd gs
	make -f Makefile.dumpitem clean
	make -f Makefile.dumpitem -j8
	cd ../../
}
buildskill()
{
 echo ""
         echo "===================== scp libskills.o =============================="
 echo ""
      	 # scp game@10.68.20.122:game_final/libskill.so $GS/gs
         # md5sum $GS/gs/libskill.so
#echo ""
#	echo "============================= ant gen =============================="
#echo ""
#	cd $SKILL
#	cd gen
#	if [ ! -d skills ]; then
#		mkdir skills
#	fi
#	if [ ! -d buffcondition ]; then
#		mkdir buffcondition
#	fi
#	ant
#echo ""
#	echo "========================== gen skils =============================="
#echo ""
#	chmod a+x gen
#	./gen
#	cd ../skill
#echo ""
#	echo "======================= build libskills.o ========================="
#echo ""
#	make clean
#	make -j8
#	cd ../../
}
buildtask()
{

echo ""
	echo "======================= build libtask.o ========================="
echo ""
	cd $GS
	cd gs
	cd task
	make clean
	make lib -j8
	cd ../../../
}

builddeliver() 
{
	cd $NET

echo ""
	echo "========================== build logservice =============================="
echo ""
	cd logservice
	make clean
	make -j8
	cd ..
echo ""

echo ""
	echo "========================== build uniquenamed =============================="
echo ""
	cd uniquenamed
	make clean
	make -j8
	cd ..
echo ""

echo ""
	echo "========================== build glinkd =============================="
echo ""
	cd glinkd
	make clean
	make -j8
	cd ..
echo ""
	echo "========================== build gdeliveryd =============================="
echo ""
	cd gdeliveryd
	make clean
	make -j8
	cd ..
echo ""
	echo "========================== build gamedbd =============================="
echo ""
	cd gamedbd
	make clean
	make -j8
	cd ../../
}
buildoncelib()
{
	buildrpcgen;
	builddeliver;
	cd $NET
echo ""
	echo "========================== build libgsio.a =============================="
echo ""
	cd io
	make lib
	cd ..
	cd ..
	cd $GS
echo ""
	echo "========================== build libTrace.a =============================="
echo ""
	cd collision
	make clean
	make -j8
	cd ..
	cd ..
}
buildgs()
{
echo ""
	echo "========================== build gs =============================="
echo ""
	cd $GS
	cd gs
	make clean
	make -j8
	cd ../../
}
rebuilddeliver()
{
	buildrpcgen;
	builddeliver;
}
rebuildlibgs()
{
	buildrpcgen;
	buildgslib;
	# buildskill;
	buildtask;
	buildgs;
}
rebuildgs()
{
	buildgs;
}
rebuildall()
{
	buildrpcgen;
	builddeliver;
	buildgslib;
	# buildskill;
	buildtask;
	buildgs;

}
if [ $# -gt 0 ]; then
	if [ "$1" = "del" ]; then
		rebuilddeliver;
	elif [ "$1" = "libgs" ]; then
		rebuildlibgs;
	elif [ "$1" = "gs" ]; then
		rebuildgs;
	elif [ "$1" = "all" ]; then
		rebuildall;
	elif [ "$1" = "first" ]; then
		buildoncelib;
		rebuildall;
	elif [ "$1" = "gslib" ]; then
		buildgslib;
	elif [ "$1" = "dumpitem" ]; then
		builddumpitem;
	fi
fi

BUILD_END_TS=$(date +%s)
BUILD_DURATION=$((BUILD_END_TS - BUILD_START_TS))
echo ""
echo "=================================================================="
echo " BUILD ENDED  : $(date '+%Y-%m-%d %H:%M:%S')"
echo " DURATION     : ${BUILD_DURATION} seconds"
echo " EXIT CODE    : $?"
echo "=================================================================="
echo ""

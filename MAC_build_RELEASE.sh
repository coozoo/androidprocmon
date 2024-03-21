#!/bin/bash

QT_DIR=/usr/local/Cellar/qt@5/
QT_DIR="$QT_DIR""$(ls $QT_DIR)"
QMAKE=$QT_DIR/bin/qmake
MAC_DEPLOY_TOOL=$QT_DIR/bin/macdeployqt

appget="$(cat *.pro |grep 'TARGET ='|awk -F\= '{print $2;}'| tr -d ' ')"

BIN_DIR="./build-$appget-Release/"
APP_NAME=$appget
BUNDLE_NAME=$APP_NAME.app
VOL_NAME="$appget"

#DMG_DIR=${APP_NAME}_dmg
#DMG_PATH=./$APP_NAME.dmg

PROJECT_FILE=$(basename -- "*.pro")

main(){
    cd "${0/*}";
    compileProject

    RESULT=$?
    if [ $RESULT -ne 0 ] ; then
        echo Failed to CompileProject
        return $RESULT
    fi

    prepareBundle
    RESULT=$?
    if [ $RESULT -ne 0 ] ; then
        echo Failed to Prepare bundle
        return $RESULT
    fi
    
    app_ver=$(cat main.cpp |grep 'const QString APP'|awk -F\" '{print $2;}')
    makeDMG
    RESULT=$?
    if [ $RESULT -ne 0 ] ; then
        echo Failed to Make DMG file
        return $RESULT
    fi

    makeZIP
    RESULT=$?
    if [ $RESULT -ne 0 ] ; then
        echo Failed to Make ZIP file
        return $RESULT
    fi

    return 0
}

compileProject(){
    echo Compiling...
    
    $QMAKE $PROJECT_FILE -config release
    
    RESULT=$?
    if [ $RESULT -ne 0 ] ; then
        echo qmake failed, error code $RESULT
        return $RESULT
    fi

    make CXX="g++"
    RESULT=$?
    if [ $RESULT -ne 0 ] ; then
        echo make failed, error code $RESULT
        return $RESULT
    fi

    echo Done.
    return 0
}

prepareBundle(){
    CURRENT_DIR=$PWD
    echo Preparing Bundle...
    
    cd $BIN_DIR
    if [ ! -d $BUNDLE_NAME ]
    then
        echo "$BUNDLE_NAME bundle doesn't exist."
        return 1
    fi

    
    # cd to Resources folder
    cd $BUNDLE_NAME
    cd Contents
    cd MacOS
    # copy appconfig to resources  
    mkdir ./lang
    cp ../../../*.qm ./lang/
    #cp ../../../chart_rules.json .
    #cp ../../../exec_history .
    #cp ../../../filters_list .
    #path to adb in my case installed by: $ brew install android-platform-tools
    #cp /usr/local/Cellar/android-platform-tools/25.0.3/bin/adb .
    #cd ..
    #cd Contents
    #cd Resources

    cd ../../../

    # Add QtFramework libraries and required plugins into .app bundle. Update dependencies of binary files.
    $MAC_DEPLOY_TOOL $BUNDLE_NAME -verbose=3

    RESULT=$?
    if [ $RESULT -ne 0 ] ; then
        echo macdeployqt failed, error code $RESULT
        return $RESULT
    fi
    cd $CURRENT_DIR
    
    curl -L https://dl.google.com/android/repository/platform-tools-latest-darwin.zip --output platform-tools-latest-darwin.zip
    if [ $? -ne 0 ]; then
	echo "unable to download adb"
    else
	unzip platform-tools-latest-darwin.zip
	if [ $? -ne 0 ]; then
	    echo "unable unpack adb"
	else
	    cp platform-tools/adb $BUNDLE_NAME/Contents/MacOS/
	    if [ $? -eq 0 ]; then
		echo "adb included"
	    else
		echo "package packed without adb"
	    fi
	fi
    fi
    
    echo Done.
     
    return 0
}

makeDMG(){
    echo Making DMG File...

    if [ ! -d "$BUNDLE_NAME" ]
    then
        echo "$BUNDLE_NAME bundle doesn't exist."
        return 1
    fi
    
    rm -f "$APP_NAME"_"$app_ver".dmg

    hdiutil create -format UDZO -srcfolder "$BUNDLE_NAME" "$APP_NAME"_"$app_ver".dmg
    
    echo Done.
    return 0
}

makeZIP(){
    echo Making ZIP File...

    if [ ! -d "$BUNDLE_NAME" ]
    then
        echo "$BUNDLE_NAME bundle doesn't exist."
        return 1
    fi
    
    rm -f "$APP_NAME"_"$app_ver".zip

    zip -r -X "$APP_NAME"_"$app_ver".zip "$BUNDLE_NAME"
    
    echo Done.
    return 0
}

main
exit $?


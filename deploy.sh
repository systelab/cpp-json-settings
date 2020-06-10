#!/bin/bash

function checkErrors
{
	if [ $? != 0 ]
	then
		echo 
		echo "An error occured while deploying artifacts."
		echo
		exit 1
	fi
}

function parseParameters
{
	VERSION="None"
	while getopts "hv:" OPTION
	do
		case $OPTION in
		h)
			usage #help: show usage section
			exit 0
			;;
		v)
			VERSION=$OPTARG
			;;
		?)
			echo "Invalid parameter" 
			usage
			exit 1
			;;
		esac
	done
	
	echo "********** DEPLOY PARAMETERS **********"
	echo "VERSION: $VERSION"
}

parseParameters "${@}"

echo -e "\nUploading JSONSettings/$VERSION@systelab/stable to systelab-bintray...\n\n"
conan upload "JSONSettings/$VERSION@systelab/stable" --all -r systelab-bintray --force
checkErrors

echo -e "\nUploading JSONSettingsTestUtilities/$VERSION@systelab/stable to systelab-bintray...\n\n"
conan upload "JSONSettingsTestUtilities/$VERSION@systelab/stable" --all -r systelab-bintray --force
checkErrors

echo -e "\nUploading JSONSettingsRESTAPI/$VERSION@systelab/stable to systelab-bintray...\n\n"
conan upload "JSONSettingsRESTAPI/$VERSION@systelab/stable" --all -r systelab-bintray --force
checkErrors

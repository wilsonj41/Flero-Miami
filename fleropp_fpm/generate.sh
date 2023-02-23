#! /bin/sh
main() {
	RED='\033[0;31m'
	NC='\033[0m'
	bold=$(tput bold)
	normal=$(tput sgr0)

	GET=""
	POST=""
	DELETE=""
	PUT=""
	while getopts "gpud" opt; do
		case "${opt}" in
		  g) GET="-g" 
		     ;;
		  p) POST="-p"
		     ;;
		  u) PUT="-u"
		     ;;
		  d) DELETE="-d" 
		     ;;
		  h | *) usage && exit 1 ;;
		esac
	done
	shift "$(($OPTIND -1))"
	for i in "$@"; do
		init_scripts/init.sh $GET $POST $PUT $DELETE $i
	done

}
usage () {
	cat <<EOF
usage: ${bold}$(basename ${0})${normal} [-gpud] filename

Tool to generate Fleropp Boilerplate, if no arguments are applied
GET function will be generated.

NOTE: if flag is used, only the flags will be generated

optional arguments:
	${bold}-g${normal}  Generate GET function
	${bold}-p${normal}  Generate POST function
	${bold}-u${normal}  Generate PUT function
	${bold}-d${normal}  Generate DELETE function
EOF
}
main $@

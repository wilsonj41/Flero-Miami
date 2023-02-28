#!/usr/bin/env bash
main() {
	FILE_NAME=""
	GET=""
	POST=""
	DELETE=""
	PUT=""
	[ "${#}" -eq 0 ] && usage && exit 1
	while getopts "gpudf:" opt; do
		case "${opt}" in
		  g) GET="1" ;;
		  p) POST="1" ;;
		  u) PUT="1" ;;
		  d) DELETE="1" ;;
		  f) FILE_NAME=$OPTARG
		esac
	done

	if [ $OPTIND -eq 1 ]; then 
		FILE_NAME="$1"
		GET="1"
	fi

	[ "$FILE_NAME" == "" ] && echo "Filename not filled" && exit 1


	cat <<EOF > $FILE_NAME.c 
#include <cgicc/HTMLClasses.h>
#include <fleropp/HTMLStream.hpp>
#include <fleropp/HTMLLiterals.hpp>

#include "$FILE_NAME.h"

INIT_VIEW($FILE_NAME)

EOF
	[[ "$GET" == "1" ]] && cat <<EOF >> $FILE_NAME.c

void $FILE_NAME::get(const fleropp_io::RequestData& request) {

}
EOF

	[[ "$POST" == "1" ]] && cat <<EOF >> $FILE_NAME.c

void $FILE_NAME::post(const fleropp_io::RequestData& request) {

}
EOF
	[[ "$PUT" == "1" ]] && cat <<EOF >> $FILE_NAME.c

void $FILE_NAME::put(const fleropp_io::RequestData& request) {

}
EOF
	[[ "$DELETE" == "1" ]] && cat <<EOF >> $FILE_NAME.c

void $FILE_NAME::del(const fleropp_io::RequestData& request) {

}
EOF

	cat <<EOF > $FILE_NAME.h 
#ifndef $(echo $FILE_NAME | tr 'a-z' 'A-Z')_HPP
#define $(echo $FILE_NAME | tr 'a-z' 'A-Z')_HPP

#include <fleropp/IView.hpp>

class $FILE_NAME : public IView<$FILE_NAME> {
  public:
    $FILE_NAME() = default;
    ~$FILE_NAME() = default;
    void get(const fleropp_io::RequestDataRequest& request);
    void post(const fleropp_io::RequestDataRequest& request);
}
#endif /* $(echo $FILE_NAME | tr 'a-z' 'A-Z')_HPP */
EOF

}

usage () {
	cat <<EOF
usage: ${0} [-gpud] filename

Tool to generate Fleropp Boilerplate, if no arguments are applied
get function will be generated.

NOTE: if flag is used, only the flags will be generated

optional arguments:
	-g  Generate GET function
	-p  Generate POST function
	-u  Generate PUT function
	-d  Generate DELETE function
EOF
}
main $@


osa for counting the number of all the source code;
# example ./linesNumber.sh . 

# usage info;
USAGE="Usage:\t./linesNumber.sh .\n"
# help info;
HELP="You can count the number of lines of the specified directory by this script\n${USAGE}"
# the tmp file to save the number of lines;
COUNT_TMP_FILE=/tmp/count
# this filter can get all the *.cpp *.c *.h files from your source code tree;
REG='\.cpp$|\.c$|\.h$'

# show usage when you use this script with some incorrect way;
show_usage(){
	    echo -ne ${HELP}
}

# function to count lines of all your source code tree;
count_lines(){
	    # excluding the error;
	    if [ $# -lt 1 ]
			    then
					        echo "Usage: count_lines $1"
							        return 1
									    fi
										    # start to count the number of lines;
										    _DIR=$1
											    echo "Start to count the number of lines in current directory ${_DIR}"
												    find ${_DIR} -name "*" | grep -E ${REG} | \
													    while read LINE
															    do
																	        NUMLINES=`cat ${LINE} | wc -l`
																				        echo ${LINE}:${NUMLINES}
																        NUM=`expr ${NUM} + ${NUMLINES}`
																			        echo ${NUM} > ${COUNT_TMP_FILE}
																		    done
																				    echo "The number of all the lines are:" 
																					    cat ${COUNT_TMP_FILE}
																			    rm -f ${COUNT_TMP_FILE}
																				    echo "Counting Finished"
}

# main;
main(){
	    if [ ! $# = 1 ]
			    then
					        # show help about this script;
					        show_usage
							        exit 1
									    else
											        # start to count lines;
											        count_lines $1
														    fi
}

# call main;
main $1


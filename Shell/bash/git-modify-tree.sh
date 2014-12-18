#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SCRIPT=`basename ${0}`
export ABSPATH="${DIR}/${SCRIPT}"

if [ -z ${GIT_COMMIT} ] ; then
  git filter-branch --tree-filter '${ABSPATH}'
else
  echo
  echo "  Rewriting ${GIT_COMMITTER_EMAIL} from ${GIT_COMMITTER_DATE}"
  FILE="fileadmin/site/db/database.sql.gz"
  if [ -f "${FILE}" ] ; then
    echo "  `ls -la fileadmin/site/db`"
    gzip -d ${FILE}
    echo "  `ls -la fileadmin/site/db`"
  else
    echo "  Not there yet."
  fi
fi

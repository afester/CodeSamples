#include <sqlite3.h>
#include <string.h>
#include <stdio.h>

static void error(const char* func, int err){
   char* errStr = "";

   switch(err) {
      case SQLITE_ERROR : errStr = " (Generic Error)"; break;
      case SQLITE_DONE  : errStr = " (Operation Completed)"; break;
      case SQLITE_BUSY  : errStr = " (Busy)"; break;
      case SQLITE_MISUSE : errStr = " (Misuse)"; break;
   }
   printf("ERROR: %s returned %d%s\n", func, err, errStr); 
}

int main() {
   sqlite3* conn = NULL;
   sqlite3_stmt* stmt = NULL;
   int err = 0;
   char* sql = NULL;
   const unsigned char * col1 = NULL;
   const unsigned char * col2 = NULL;

   /* Open database */
   err = sqlite3_open("sample.dbf", &conn);
   if (err != SQLITE_OK) {
      error("sqlite3_open", err);
   }

   /* Drop table and ignore error when table does not exist */
   sql = "DROP TABLE mytable";
   err = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
   if (err != SQLITE_OK && err != SQLITE_ERROR) {	/* ignore non existing table */
      error("sqlite3_prepare", err);
   } else {
      err = sqlite3_step(stmt);
      if (err != SQLITE_DONE) {
         error("sqlite3_step", err);
      }
      err = sqlite3_finalize(stmt);
      if (err != SQLITE_OK) {
         error("sqlite3_finalize", err);
      }
   }

   /* Create table */
   sql = "CREATE TABLE mytable (parentId TEXT, childId TEXT)";
   err = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
   if (err != SQLITE_OK) {
      error("sqlite3_prepare", err);
   }
   err = sqlite3_step(stmt);
   if (err != SQLITE_DONE) {
      error("sqlite3_step", err);
   }
   err = sqlite3_finalize(stmt);
   if (err != SQLITE_OK) {
      error("sqlite3_finalize", err);
   }

   sql = "INSERT INTO mytable VALUES('A', 'B')";
   err = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
   if (err != SQLITE_OK) {
      error("sqlite3_prepare", err);
   }
   err = sqlite3_step(stmt);
   if (err != SQLITE_DONE) {
      error("sqlite3_step", err);
   }
   err = sqlite3_finalize(stmt);
   if (err != SQLITE_OK) {
      error("sqlite3_finalize", err);
   }

   sql = "INSERT INTO mytable VALUES('C', 'D')";
   err = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
   if (err != SQLITE_OK) {
      error("sqlite3_prepare", err);
   }
   err = sqlite3_step(stmt);
   if (err != SQLITE_DONE) {
      error("sqlite3_step", err);
   }
   err = sqlite3_finalize(stmt);
   if (err != SQLITE_OK) {
      error("sqlite3_finalize", err);
   }

   sql = "SELECT * FROM mytable";
   err = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
   if (err != SQLITE_OK) {
      error("sqlite3_prepare", err);
   }

/******************************************/
   {
      sqlite3_stmt* stmt = NULL;
      char* sql = NULL;

      sql = "PRAGMA table_info(mytable)";
      err = sqlite3_prepare_v2(conn, sql, -1, &stmt, NULL);
      if (err != SQLITE_OK) {
         error("sqlite3_prepare", err);
      }
      err = sqlite3_step(stmt);
      if (err != SQLITE_DONE) {
         error("sqlite3_step", err);
      }
      err = sqlite3_finalize(stmt);
      if (err != SQLITE_OK) {
         error("sqlite3_finalize", err);
      }
   }
/******************************************/


   do {
      err = sqlite3_step(stmt);
      if (err != SQLITE_DONE && err != SQLITE_ROW) {
         error("sqlite3_step", err);
      }

      if (err != SQLITE_DONE) {
        col1 = sqlite3_column_text(stmt, 0);
        col2 = sqlite3_column_text(stmt, 1);
        printf("(%s, %s)\n", col1, col2);
      }
   } while(err == SQLITE_ROW);

   err = sqlite3_finalize(stmt);
   if (err != SQLITE_OK) {
      error("sqlite3_finalize", err);
   }


   /* Close database */
   err = sqlite3_close(conn); 
   if (err != SQLITE_OK) {
      error("sqlite3_close", err);
   }

   return 0;
}

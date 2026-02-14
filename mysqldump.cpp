#include <iostream>
#include <fstream>
#include <string>
#include "vendor/json.hpp"
#include <regex>
#include <vector>
#include <mysql.h>
#include <thread>
#include <chrono>

using json = nlohmann::json;

using namespace std;

class connector{

	private:
		string path;
		json data;

		struct User {
			string nim;
			string nama;
		};
		vector<User> hasil_dump;

		void coba(MYSQL* conn, string nama, string nim, int diskon, string email, string kode_promo, string kode_redeem) {
			mysql_query(conn, "START TRANSACTION");
			string q1 =
				"INSERT INTO promo "
        		"(kode, diskon, tanggal_dibuat, status_promo, max_usage, current_usage) VALUES "
        		"('" + kode_promo + "', " + to_string(diskon) +
        		", CURDATE(), 'ACTIVE', 1, 1)";
			
			if (mysql_query(conn, q1.c_str())) {
				cout << mysql_error(conn) << endl;
				mysql_query(conn, "ROLLBACK");
				return;
			}


			string q2 =
        		"INSERT INTO transactions "
        		"(nim_temp, payment_status, kode_promo, nama_lengkap_temp, email_temp, angkatan_temp, nominal) VALUES "
        		"('" + nim + "', 'SUCCESS', '" + kode_promo + "', '" +
        		nama + "', '" + email + "', '2024', 32000)";

    		if (mysql_query(conn, q2.c_str())) {
       			 cout << mysql_error(conn) << endl;
        		mysql_query(conn, "ROLLBACK");
        		return;
   			 }

    		long long trx_id = mysql_insert_id(conn);


		    string q3 =
        		"INSERT INTO mahasiswa "
        		"(nim, nama_lengkap, email, angkatan, transactions_id) VALUES "
        		"('" + nim + "', '" + nama + "', '" + email + "', '2024', "
        		+ to_string(trx_id) + ")";

    		if (mysql_query(conn, q3.c_str())) {
        		cout << mysql_error(conn) << endl;
        		mysql_query(conn, "ROLLBACK");
        		return;
    		}


   			 string q4 =
        		"INSERT INTO ticket "
        		"(kode_redeem, isVIP, transactions_id) VALUES "
        		"('" + kode_redeem + "', 1, "
        		+ to_string(trx_id) + ")";

    		if (mysql_query(conn, q4.c_str())) {
        		cout << mysql_error(conn) << endl;
        		mysql_query(conn, "ROLLBACK");
        		return;
    		}


    		string q5 =
        		"INSERT INTO photo_booth "
        		"(waktu_photo_booth, kode_redeem) VALUES "
        		"(NOW(), '" + kode_redeem + "')";

    		if (mysql_query(conn, q5.c_str())) {
        		cout << mysql_error(conn) << endl;
        		mysql_query(conn, "ROLLBACK");
        		return;
    		}

   			 mysql_query(conn, "COMMIT");
			 cout << "SEMUA BERHASIL!" << endl;
		}

		
	public:
		bool ifexists(string pathcheck){
			ifstream f(pathcheck.c_str());
			return f.good();
		}

		connector(string cur_path){
			path = cur_path;
			if (!ifexists(path)){
				cout << "gk ada boy" << endl;
				return;
			} try {
				ifstream f(path);
				data = json::parse(f);
			} catch(exception e){
				cout << "anjg goblok" << data << endl;
			}
		}
		void push_vector(){
			cout <<  data["users"].size() << endl;
			regex pattern("(\\d+)"); 
			smatch match;
			int i = 0;

			MYSQL* conn;
			conn = mysql_init(0);
			conn = mysql_real_connect(conn, "localhost", "root", "", "igate", 3306, NULL, 0);

			if (conn) {
				cout << "CONNECTED TO MY SQL" << endl;
			} else {
				cout << "Gagal konek: " << mysql_error(conn) << endl;
				return;
			}

			for(int i = 0; i < data["users"].size(); i++){
			
				string usn = data["users"][i]["fullname"];
				
				if (regex_search(usn, match, pattern)) {
					string nim = match[0]; 
					string nama = match.prefix();
					if(nim[0] == '2' && nim[1] == '4' && nim[2] == '0' && nim[3] == '6' && nim[7] == '4'){
					
						hasil_dump.push_back({nim, nama});
					}
					
				}
			}
			for(int i = 0; i < hasil_dump.size(); i++){
				long long timestamp = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

				 long long last5 = (timestamp + i) % 100000; //biar gk rebutan
				 std::this_thread::sleep_for(std::chrono::milliseconds(500));
				cout << hasil_dump[i].nim << endl;
				cout << hasil_dump[i].nama << endl;
				cout << "tryint to input " << hasil_dump[i].nim << " " << hasil_dump[i].nama << " to igate" << " " << last5 << endl;
				//https://github.com/galviy/mysql-dummy-data-maker/blob/main/jajajaja.png?raw=true
				coba(conn,hasil_dump[i].nama, hasil_dump[i].nim, 20,hasil_dump[i].nim + "@students.undip.ac.id","PROMO2026" + to_string(last5),"TICKET2026ABC" + to_string(last5));
			}
		}
};

int main(){

	connector p("data.json");
	p.push_vector();
	
}
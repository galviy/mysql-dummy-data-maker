# MYSQL Dummy Data maker

## what is this program use to??

so yeah it just useless mysql dummy data maker for my college task and stuff, you can adjust the query as u need. The point is iam not inserting 100 data /table using my bare hand lmao ;D

![Cool Thing](https://github.com/galviy/mysql-dummy-data-maker/blob/main/jajajaja.png?raw=true)


## how to build


```cmake
cmake --build .
```

## Modifying data source
Adjust to your own json struct & filtering logic here

```c++
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
```

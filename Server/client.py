from flask.globals import request
import requests

Host = "http://127.0.0.1"
Port = "5000"
url = Host + ":" + Port + "/"


f_name = "ahmed"
l_name = "mohamed"
username = "ahmed_mohamed"
password = "12345"
serial = "8wUy6"
email = "ahmed_mohamed@testttt.com"
physical_id = "rskDSkocuB6YuLxO"


# worked fine 
#response = requests.post(url+"/reg", json={"first_name":f_name, "last_name":l_name, "email":email, "username":username, "password":password, "serial":serial})
#print(response.json())
#input()

 
def simple_encrypt(key, val):
    ret = ""
    for i in range(16):
        ret += chr(1+ord(val[i]))
    return ret

response = requests.post(url+"login",json={"password":password, "email":email})
res = response.json()
encrpt = simple_encrypt(physical_id, res['challenge'])
#print(encrpt)
#print(res)
input()

response = requests.post(url+"2fa",json={"Access-token":res['Access-token'], "challenge":encrpt})
result= response.json()

#print(result)
 

input()
# add account 
response = requests.post(url+"Add-acc",json={'url':'youtube.com', 'username':'youtube_user2', 'password':'youtube_password', 'Access-token': result['Access-token']})
print(response.json())

# view only urls 
response = requests.post(url+"view-urls", json={'Access-token': result['Access-token']})
print(response.json())

input()
#view all accounts associated with specific url 
response = requests.post(url+"view-accounts", json={ "url":'youtube.com', 'Access-token': result['Access-token']})
print(response.json())

#response = requests.post(url+"delete-acc", json={ "url":'youtube.com',"username":'youtube_user1', 'Access-token': result['Access-token']})
#print(response.json())

response = requests.post(url+"update", json={ "url":'youtube.com',"old_username":'youtube11_user1', "username":"new_user1","password":"pass",'Access-token': result['Access-token']})
print(response.json())




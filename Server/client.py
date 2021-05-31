import requests

Host = "http://127.0.0.1"
Port = "5000"
url = Host + ":" + Port + "/"


f_name = "fname"
l_name = "lname"
username = "username"
password = "1234"
physical_id = "testtoken123"
email = "test@testttt.com"



# worked fine 
response = requests.post(url+"reg", {"first_name":f_name, "last_name":l_name, "email":email, "username":username, "password":password, "physical_id":physical_id})
print(response.json())
input()

# 
def simple_encrypt(key, val):
    ret = key+val
    return ret 

response = requests.post(url+"login",{"password":password, "email":email})
res = response.json()
encrpt = simple_encrypt(physical_id, res['challange'])
print(encrpt)
print(res)
input()


response = requests.post(url+"2fa",{"Access-token":res['Access-token'], "challange":encrpt})
res = response.json()
# 
print(res)



input()
# add account 
# response = requests.post(url+"Add-acc",{'url':'google.com', 'username':'google_user', 'password':'google_password', 'Access-token': res['Access-token']})
# print(response.json())

# view only urls 
# response = requests.post(url+"view-urls", { 'Access-token': res['Access-token']})
# print(response.json())

input()
# view all accounts associated with specific url 
# response = requests.post(url+"view-accounts", { "url":'google.com', 'Access-token': res['Access-token']})
# print(response.json())






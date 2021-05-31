from flask import Flask, request, jsonify
import uuid, string, random
from api import mongo , app , is_token_exist, SECRET_KEY, get_user_by_email
from flask import Blueprint
from functools import wraps
import jwt , datetime
import json
import base64
import pyaes
import binascii

status_code_ok = "200"
status_code_fail = "400"
login_blueprint = Blueprint('login', __name__)
@login_blueprint.route('/login', methods=['POST'])
def initial_login():
    f = open("log/log_login.txt", "a")
    n = f.write(str(datetime.datetime.now()) + ": Login - " + str(request.json) + "\n")
    f.close()
    
    args = request.json
    email = args.get('email')
    password = args.get('password')
    id = str(uuid.uuid4())

    challenge = ''.join(random.choices(string.ascii_uppercase + string.digits, k = 16))
    mongo.db.session.insert_one({'id':id ,'email': email, 'password': password, 'challenge':challenge})
    
    timelimit = datetime.datetime.now() + datetime.timedelta(seconds=1000)
    payload = {"user_id":id, "exp":timelimit}
    jwt_token = jwt.encode(payload,SECRET_KEY)
 
    return_info = {
     "code":status_code_ok,
     "Access-token":jwt_token.decode("UTF-8"),
     "challenge":challenge
    }
    return return_info

@login_blueprint.route("/2fa", methods=['POST'])
def authentication():
    f = open("log/log_2fa.txt", "a")
    n = f.write(str(datetime.datetime.now()) + ": 2FA - " + str(request.json) + "\n")
    f.close()

    req = request.json
    token_exist, token = is_token_exist(req)
    if token_exist == True:
        user_id = token['user_id']
   	#return {"Message":"1"}
    else: return {"code":status_code_fail}
    encrpted_challange = req.get('challenge')
    finduser = mongo.db.session.find_one({'id':user_id})
 
    if finduser:
        ret = User_login(user_id,finduser['email'], finduser['password'], finduser['challenge'], encrpted_challange)
        return ret
    else:return {"code":status_code_fail} 

def encrpyt_chall(key, plaintext):
    iv = "AAAAAAAAAAAAAAAA" #IV for CBC
    aes = pyaes.AESModeOfOperationCBC(bytes(key.encode('utf-8')), iv = bytes(iv.encode('utf-8')))
    ciphertext = aes.encrypt(bytes(plaintext.encode('utf-8')))
    return binascii.hexlify(ciphertext).decode('ascii').upper()

def get_physical_id(public_id):
    find_physicalID = mongo.db.devices.find_one({'Serial':public_id})
    return find_physicalID

def User_login(user_id,email, password, challenge, encrpted_challange):

    encrpted_challange = base64.b64decode(encrpted_challange.encode('ascii').decode('ascii'))
    # print(encrpted_challange)
    #return {"code":"ok"}
    current_user = get_user_by_email(email)
    if current_user:
        public_id = current_user['public_id']
        physical = get_physical_id(public_id)
        physical_key = physical['Physical_id']
        
        original_encrpted = encrpyt_chall( physical_key, challenge)
        f = open("log/log_tokens.txt", "a")
        n = f.write(str(datetime.datetime.now()) + " -> Original: " + original_encrpted + " - ")
        n = f.write("Recieved: " + encrpted_challange.decode("utf-8") + "\n")
        f.close()
        #return {"original":original_encrpted, "encrypted":encrpted_challange.decode("utf-8")}

        if current_user['password'] != password or original_encrpted != encrpted_challange.decode("utf-8"):
            #return {"original":original_encrpted, "encrypted":encrpted_challange.decode("utf-8")}
            return  {"code": status_code_fail}
    else:
            return  {"code": status_code_fail }

    timelimit = datetime.datetime.now() + datetime.timedelta(seconds=10000)
    payload = {"user_id":public_id, "exp":timelimit}
    jwt_token = jwt.encode(payload,SECRET_KEY)


    mongo.db.session.remove({"id":user_id})


    return_info = {
	"code":status_code_ok,
        "Access-token":jwt_token.decode("UTF-8"),
    }
    return return_info
        
    


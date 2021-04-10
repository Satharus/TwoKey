from flask import Flask, request, jsonify
import uuid, string, random
from api import mongo , app, get_user_by_email
from flask import Blueprint
# from functools import wraps
import jwt , datetime
reg_blueprint = Blueprint('register', __name__)


def serial_exist(serial):
    serial = mongo.db.devices.find_one({'Serial':serial})
    if not serial:
        return False
    else: 
        return True
@reg_blueprint.route("/reg", methods=['POST'])
def User_Register():
    #return {"message":"ok"}
    # f = open("log/log_register.txt", "a")
    # n = f.write(str(datetime.datetime.now()) + ": Register - " + str(request.json) + "\n")
    # f.close()

    args = request.json
    # print (args)
    firstname = args['first_name']
    # return {"msg":"ok"}, 200
    lastname = args.get('last_name')
    email = args.get('email')
    username = args.get('username')
    password = args.get('password')
    serial = args.get('serial')
    public_id = str(uuid.uuid4())
   
    if not serial_exist(serial):
        return {"Message":"Wrong Serial!"}

    if get_user_by_email(email):
        return {"Message":"User already exists"}
    
    user = mongo.db.user.insert_one({"public_id":public_id, 'last_name':lastname,
                                          "first_name":firstname, "email":email,
                                          "username":username, "password":password})

    mongo.db.create_collection(public_id)
    mongo.db.devices.update_one({"Serial":serial}, {"$set": {"Serial":public_id}})
    #db.coll.updateOne({"_id": 1, "grades": 80}, {$set: {"grades.$": 82}})

    return {"Message":f"Done {firstname}"}, 200












    

    

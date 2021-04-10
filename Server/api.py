from flask import Flask, request, jsonify, session
from bson.json_util import dumps
from flask_restful import Api, Resource, reqparse, abort, fields, marshal_with
# from flask_mongoengine import MongoEngine
from flask_pymongo import PyMongo
import uuid, string, random
from functools import wraps
import jwt , datetime

app = Flask(__name__)

SECRET_KEY = "secret_key"
app.config["MONGO_URI"] = "mongodb://localhost:27017/testdb"

mongo = PyMongo(app)

@app.route("/")
def hello():
    return {"message":"ok"},200
	# return "<h1 style='color:red'>hello thereeee!</h1>"

def get_user_by_email(email):
    finduser = mongo.db.user.find_one({'email':email})
    return finduser

def is_token_exist(req):
    token_data = None
    jwt_token = req.get('Access-token')
    if not jwt_token:
        return False, ({"Message" : "Token is missing!!"})
    try:
        token_data = jwt.decode(jwt_token, SECRET_KEY)
    except:
        return False, ({"Message": "Invalid Token!!"})
    return True, token_data    


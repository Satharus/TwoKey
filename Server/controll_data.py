from flask import Flask, request, jsonify
from api import mongo , app , is_token_exist
from flask import Blueprint
import json 

control_blueprint = Blueprint("controll_data", __name__)
status_code_ok = "200"
status_code_fail = "400"
def get_all_user_data(req):
    token_exist, token = is_token_exist(req)
    if token_exist == True:
        user_id = token['user_id']
        data = mongo.db[user_id].find()
    else: 
        return 0
    return data

@control_blueprint.route("/view-urls", methods=['POST'])
def view_only_urls():
    args = request.json
    data = get_all_user_data(args)
    if data:
        all_urls = []
        for account in data:
            if account['url'] not in all_urls:
                all_urls.append(account['url'])
    else:
        return {"code":status_code_ok}
    data = {
        "code":status_code_ok,
        "data":json.dumps(all_urls),
    }
    return data

@control_blueprint.route("/view-accounts", methods=['POST'])
def View_accounts_by_url():
    req = request.json
    data = get_all_user_data(req)
    url = req.get('url')
    if data:
        accounts = []
        for account in data:
            if account['url'] == url:
                l = []
                l.append(account['username'])
                l.append(account['password'])
                accounts.append(l)
    else:
        {"code":status_code_fail}
    data = {
        "code":status_code_ok,
        "data":json.dumps(accounts),
    }
    return data
@control_blueprint.route("/Add-acc", methods=['POST'])
def Add_account():
    req = request.json
    token_exist, token = is_token_exist(req)
    user_data = get_all_user_data(req)
  
    if token_exist == True:
        user_id = token['user_id']
        print(user_id)
        url = req.get("url")
        username = req.get('username')
        password = req.get('password')

        if not url or not username or not password:
            return {"code":status_code_fail}
        username_duplicated = 0
        if user_data:
            accounts = []
            for account in user_data:
                if account['url'] == url and account['username'] == username:
                    username_duplicated = 1
        if username_duplicated == 0:
            data = mongo.db[user_id].insert_one({"url":url,
                                            "username":username,
                                            "password":password})
        else:
            return {"code":status_code_fail}
    else:
    	data = {
        "code":status_code_ok,
        "data":token,
    	}
        return data
    return {"code":status_code_fail}

@control_blueprint.route("/delete-acc", methods=['POST'])
def delete_specific_account():
    req = request.json
    token_exist, token = is_token_exist(req)
    if token_exist == True:
        user_id = token['user_id']
        url = req.get("url")
        username = req.get("username")
        if not url or not username:
            return {"code":status_code_fail}
        else:
            data = mongo.db[user_id].find_one_and_delete({"url":url, "username":username})
            if not data:
                return {"code":status_code_fail}
    return {"code":status_code_ok}
@control_blueprint.route("/update", methods=['POST'])
def update_specific_account():
    req = request.json
    token_exist, token = is_token_exist(req)
    if token_exist == True:
        user_id = token['user_id']
        url = req["url"]
        old_username = req["old_username"]
        username = req['username']
        password = req['password']

        user_data = get_all_user_data(req)
        if not url or not username or not old_username or not password:
            return {"code":status_code_fail}
        else:
            exist = 0
            if user_data:
                accounts = []
                for account in user_data:
                    if account['url'] == url and account['username'] == old_username:
                        exist = 1
            if exist == 1:
                mongo.db[user_id].update_one({"username":old_username}, {"$set": {"username":username, "password":password}})
            else:
                return {"code":status_code_fail}
    return {"code":status_code_ok}

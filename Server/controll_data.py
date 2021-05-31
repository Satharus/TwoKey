from flask import Flask, request, jsonify
from api import mongo , app , is_token_exist
from flask import Blueprint
import json 

control_blueprint = Blueprint("controll_data", __name__)

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
        return {"Message":"No accounts yet.."}
    return json.dumps(all_urls)

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
        {"Message":f"No accounts for {url} yet.."}
    return json.dumps(accounts)
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
            return {"Message":"Url, username or password is missing !!"}
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
            return {"code":400}
    else:
        return token
    return {"Message":f"your data to {url} is saved..!"}

@control_blueprint.route("/delete-acc", methods=['POST'])
def delete_specific_account():
    req = request.json
    token_exist, token = is_token_exist(req)
    if token_exist == True:
        user_id = token['user_id']
        url = req.get("url")
        username = req.get("username")
        if not url or not username:
            return {"Code":400}
        else:
            data = mongo.db[user_id].find_one_and_delete({"url":url, "username":username})
            if not data:
                return {"Code":400}
    return {"Code":200}
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
            return {"Codeee":400}
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
                return {"code":400}
    return {"Code":200}

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
    if token_exist == True:
        user_id = token['user_id']
        url = req.get("url")
        username = req.get('username')
        password = req.get('password')
        if not url or not username or not password:
            return {"Message":"Url, username or password is missing !!"}
        data = mongo.db[user_id].insert_one({"url":url,
                                            "username":username,
                                            "password":password})
    else:
        return token
    return {"Message":f"your data to {url} is saved..!"}

from register import *
from api import *
from flask import Flask
from login import *
from controll_data import *

app.register_blueprint(reg_blueprint)
app.register_blueprint(login_blueprint)
app.register_blueprint(control_blueprint)

if __name__ == "__main__":
#debug = true is to see the errors just for
#testing but not good in production
    app.run(debug=True)

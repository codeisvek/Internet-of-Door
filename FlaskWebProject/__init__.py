from flask import Flask, request, redirect
from twilio.rest import TwilioRestClient
 
# Your Account Sid and Auth Token from twilio.com/user/account
account_sid = "SID"
auth_token  = "TOKEN"
client = TwilioRestClient(account_sid, auth_token)
 
app = Flask(__name__)

application = 0

@app.route('/', methods=['GET', 'POST'])
def hello_world():
    temp = application
    if application != 0:
        global application
        application = 0   
    return str(temp)

@app.route('/door')
def ring():
    message = client.messages.create(body="Someone's at the door. Let them in?",
    to="+17149473284",    # Replace with your phone number
    from_="15624441271") # Replace with your Twilio number
    print message.sid
    return 'swag!'



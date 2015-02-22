from flask import Flask, request, redirect
from twilio.rest import TwilioRestClient
 
# Your Account Sid and Auth Token from twilio.com/user/account
account_sid = "AC32a3c49700934481addd5ce1659f04d2"
auth_token  = "9702a9d7f685c995868eaee46495c792"
client = TwilioRestClient(account_sid, auth_token)
 
app = Flask(__name__)

application = 0

@app.route('/')
def hello_world():
    message = client.messages.create(body="Jenny please?! I love you <3",
    to="+17149473284",    # Replace with your phone number
    from_="15624441271") # Replace with your Twilio number
    print message.sid
    return 'swag!'



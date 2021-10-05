from flask import Flask, abort, render_template, request
from wtforms import *
from datetime import datetime
import json

app = Flask(__name__)
default_date = datetime.now().strftime("%Y-%m-%d")

with open(r"users.json", 'r') as f:
    users = json.load(f)


class RegisterForm(Form):
    fname = simple.StringField(
        label="First Name",
        validators=[
            validators.DataRequired(message="First Name could not be empty")
        ],
        widget=widgets.TextInput(),
        render_kw={"class": "form-control"}
    )

    lname = simple.StringField(
        label="Last Name",
        validators=[
            validators.DataRequired(message="Last Name could not be empty")
        ],
        widget=widgets.TextInput(),
        render_kw={"class": "form-control"}
    )

    email = fields.StringField(
        label="Email Address",
        validators=[
            validators.DataRequired(message="Email Address could not be empty"),
            validators.Email(message="Please enter a valid email address")
        ],
        widget=widgets.TextInput(),
        render_kw={"class": "form-control"}
    )

    dob = fields.DateField(
        label="Date of Birth",
        format='%Y-%m-%d',
        default=datetime.now(),
        validators=[
            validators.DataRequired(message="Date of Birth could not be empty")
        ],
        widget=widgets.TextInput(),
        render_kw={"class": "form-control"}
    )

    def __init__(self, *args, **kwargs):
        super(RegisterForm, self).__init__(*args, **kwargs)


@app.route('/')
def index():
    return request.host


@app.route('/users/<int:user_id>')
def user_info(user_id):
    user_id = str(user_id)
    if user_id in users.keys():
        return render_template(r"user_page.html", users=users, user_id=user_id)
    else:
        abort(404)


@app.route('/users/<int:user_id>/Edit')
def edit_user_info(user_id):
    user_id = str(user_id)
    form = RegisterForm(formdata=request.form)
    if user_id in users.keys():
        return render_template('user_edit_page.html', form=form)
    else:
        abort(404)

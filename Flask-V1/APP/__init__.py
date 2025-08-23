"""初始化app对象，蓝图绑定"""
from flask import Flask
from .View import *
from .exts import *

def init():

    """初始化app对象"""
    app=Flask(__name__)

    """蓝图绑定"""
    app.register_blueprint(blueprint=blue)

    """连接SQLite数据库"""
    uri='sqlite:///book.db'
    app.config['SQLALCHEMY_DATABASE_URI']=uri

    """初始化插件"""
    init_(app)

    return app
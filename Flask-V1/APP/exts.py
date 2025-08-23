"""管理插件以及初始化"""
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate

"""初始化"""
db=SQLAlchemy()
migrate=Migrate()

"""与app对象进行绑定"""
def init_(app):
    db.init_app(app=app)
    migrate.init_app(app=app,db=db)
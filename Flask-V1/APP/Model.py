"""数据库中表的建立"""
from .exts import db

"""创建书籍表"""
class Books(db.Model):
    __tablename__ = 'books'
    """主键"""
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    """书名"""
    title = db.Column(db.String(50), nullable=False,index=True)
    """作者"""
    author = db.Column(db.String(100), nullable=False,index=True)
    """出版社"""
    publisher = db.Column(db.String(50), nullable=False)
    """ISBN"""
    isbn = db.Column(db.String(20), unique=True, nullable=True)
    """标签"""
    tip = db.Column(db.String(30), nullable=False,index=True)
    """内容简介"""
    info = db.Column(db.Text,nullable=True)
    """封面图片路径"""
    cover_image_page = db.Column(db.String(100), nullable=False)
    """PDF文件路径"""
    pdf_path = db.Column(db.String(100), nullable=False)
    """外键"""
    tip_id=db.Column(db.Integer,db.ForeignKey("tip.id",name='books_tip_id_fkey'))

"""创建tip表"""
class Tip(db.Model):
    __tablename__='tip'
    id=db.Column(db.Integer,primary_key=True,autoincrement=True)
    name=db.Column(db.String(30), nullable=False,index=True)
    data_book=db.relationship('Books',backref='tips',lazy="dynamic")

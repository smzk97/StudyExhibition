"""操作数据库，创建路由器"""
from flask import Blueprint, render_template, request, redirect, current_app
from .Model import *
import os

"""使用蓝图"""
blue=Blueprint('user',__name__)

"""定义路由器"""
@blue.route('/',methods=['POST','GET'])
@blue.route('/home/',methods=['POST','GET'])
def home():
    page=request.args.get('page',1)
    per_page=request.args.get('per_page',6)
    data=Books.query.paginate(page=int(page),per_page=int(per_page),error_out=False)
    return render_template('home.html',data=data,tip=Tip.query.all())

@blue.route('/books/',methods=['POST','GET'])
def books():
    title=request.args.get('title')
    book=Books.query.filter(Books.title == title).first()
    # 获取PDF文件大小（如果文件存在）
    pdf_path = os.path.join(current_app.static_folder, book.pdf_path)
    if os.path.exists(pdf_path):
        # 转换为MB并保留两位小数
        book.pdf_size = round(os.path.getsize(pdf_path) / (1024 * 1024), 2)
    else:
        book.pdf_size = None
    return render_template('books.html',data=book,tip=Tip.query.all())

@blue.route('/tips/',methods=['POST','GET'])
def tips():
    page = request.args.get('page', 1)
    per_page = request.args.get('per_page', 6)
    tip=request.args.get('tip',None)
    data = Tip.query.filter(Tip.name == tip).first().data_book.paginate(page=int(page), per_page=int(per_page), error_out=False)
    return render_template('tips.html',data=data,tip=Tip.query.all())

@blue.route('/search/',methods=['POST','GET'])
def search():
    data_search=request.args.get('search','').strip()
    page=request.args.get('page',1,type=int)
    per_page=request.args.get('per_page',6,type=int)
    data=Books.query.filter(Books.title.ilike(f'%{data_search}%')).paginate(page=page,per_page=per_page,error_out=False)
    return render_template('search.html',data=data,tip=Tip.query.all(),data_search=data_search)
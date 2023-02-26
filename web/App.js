// 引入express
let express = require('express')
//中间件：获取post请求传递的数据
let bodyParser = require('body-parser')
let jwt = require('jsonwebtoken')
let fs = require('fs')
let path = require('path')
// 创建应用程序
let app = express()
//指令执行的根路径
let root = process.cwd()
//定义加密字符串
let str = "whwh"
//安装中间件
app.use(bodyParser.json())
// mock数据。
// 登录请求
app.post('/login', (req, res) => {
    let { username, password } = req.body;
    // 用户登录成功后，通过jwt的sign方法将用户信息以及加密字符串捆绑生成token字符串
    let token = jwt.sign(req.body, str)
    //如果用户名和密码正确，则返回生成的token给前端
    if (username === 'admin' && password === '123') {
        return res.json({ errno: 0, token })
    }
    return res.json({ errno: 1, msg: '用户名或者密码错误' })
})
app.listen(3000, () => console.log("3000 port is listening"))

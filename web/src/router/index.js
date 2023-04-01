//路由组件控制页面
import Vue from 'vue'
import VueRouter from 'vue-router'

//1.路由组件引入
// import ScheduleMge from '../views/ScheduleMge.vue'
import CourseNav from '../views/CourseNav.vue'
import Homepage from '../views/Homepage.vue'
import inClass from '../views/inClass.vue'
import outClass from '../views/outClass.vue'
import Main from '../views/Main.vue'
import Register from '../views/Register.vue'
import Login from '../views/Login.vue'
import logMessage from '../views/logMessage.vue'
import { Message } from 'element-ui'


Vue.use(VueRouter)

//2.将路由与组件进行映射,path是网页中的路径，而component是组件的名称
const routes = [
    //主路由
    {
        path: '/Main',
        component: Main,
        //嵌套路由
        children: [
            // { path: 'ScheduleMge',component: ScheduleMge,},
            { path: 'inClass', component: inClass },
            { path: 'outClass', component: outClass },
            { path: 'CourseNav', component: CourseNav },
            { path: 'Homepage', component: Homepage },
            { path: 'logMessage', component: logMessage }
        ]
    },
    { path: '/', component: Login, },
    { path: '/Register', component: Register, },
    
]

//3.创建router实例
const router = new VueRouter({
    routes,
    mode:'history'
})

// 4.设置用户进入页面的权限, 当用户处于登录状态时, 可以直接进入其他页面, 否则会跳转到登录页面
router.beforeEach((to, from, next) => {
    if (to.path == '/' || to.path == '/Register') {
        // 登录和注册页面可以直接进入
        next()
    } else {
        if (window.localStorage.getItem('token') == null) {
            Message.warning({
                showClose: true,
                center: true,
                message: '您需要先登录！！！',
            })
            next('/')
        } else {
            next()
        }
    }
})

//5.将router实例对外暴露
export default router

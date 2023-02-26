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
//4.将router实例对外暴露
export default router

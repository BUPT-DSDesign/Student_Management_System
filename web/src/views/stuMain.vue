<template>
    <div>
        <el-container>
            <el-aside width="200px">
                <!-- 侧边栏 -->
<<<<<<<< HEAD:web/src/views/studentMain.vue
               <studentAside :routes="getRoutes"></studentAside>
========
               <stuAside :routes="getRoutes"></stuAside>
>>>>>>>> d5cbff461f541e54f1b5e0970e7b1dfe32de479c:web/src/views/stuMain.vue
            </el-aside>
            <el-container>
                <el-header>   
                    <!-- 上方 -->
                    <Header></Header>
                </el-header>
                <el-main>
                    <!-- 子路由的路由出口 -->
                    <router-view></router-view>
                </el-main>
            </el-container>
        </el-container>
    </div>
</template>
<script>
<<<<<<<< HEAD:web/src/views/studentMain.vue
import studentAside from '../components/studentAside.vue'
========
import stuAside from '../components/stuAside.vue'
>>>>>>>> d5cbff461f541e54f1b5e0970e7b1dfe32de479c:web/src/views/stuMain.vue
import Header from '../components/Header.vue'
 
export default {
    data() {
        return {
        }
    },
    computed: {
        getRoutes() {
            return global.antRouter
        }
    },
    data() {
        return {
            userRole: 'Topest',
        }
    },
    components: {
<<<<<<<< HEAD:web/src/views/studentMain.vue
        studentAside,
========
        stuAside,
>>>>>>>> d5cbff461f541e54f1b5e0970e7b1dfe32de479c:web/src/views/stuMain.vue
        Header,
    }
}
</script>
<style>
.el-header{
    padding: 0;
}
.el-main{
    padding: 0;
}
</style>


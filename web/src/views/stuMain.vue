<template>
    <div>
        <el-container>
            <el-aside width="200px">
                <!-- 侧边栏 -->
               <stuAside :routes="getRoutes"></stuAside>
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
import stuAside from '../components/stuAside.vue'
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
        stuAside,
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


<template>
    <div>
        <el-container>
            <el-aside width="200px">
               <Aside></Aside>
            </el-aside>
            <el-container>
                <el-header>   
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
import Aside from '../components/Aside.vue'
import Header from '../components/Header.vue'

export default {
    data() {
        return {
        }
    },

    // methods: {
    //     receiveData(data) {
    //         this.courseList = data
    //         console.log(this.courseList)
    //         bus.$emit('courseList', this.courseList)
    //     }
    // },

    components: {
        Aside,
        Header
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


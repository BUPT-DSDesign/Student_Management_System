<template>
    <div class="Aside">
        <el-menu default-active="1" class="el-menu-vertical-demo" @open="handleOpen" @close="handleClose"
            :collapse="isCollapse" background-color="#545c64" text-color="#fff" active-text-color="#ffd04b">
            <h3 class="title">学生日程管理系统</h3>
            <el-menu-item index="1" @click="clickClassManage">
                <i class="el-icon-user-solid"></i>
                <span slot="title">课程管理</span>
            </el-menu-item>
        </el-menu>
    </div>
</template>

<script>

export default {
    data() {
        return {
            isCollapse: false,
            
        };
    },
    methods: {
        clickClassManage() {
            this.$router.push('/adminMain/classmanage');
        },
    }
}
</script>

<style>
* {
    margin: 0;
    padding: 0;
}

html,
body {
    height: 100%;
}

.Aside {
    height: 100vh;
}

.el-menu-vertical-demo:not(.el-menu--collapse) {
    width: 200px;
    height: 100%;
}

.el-aside {
    overflow-x: hidden;
}

h3 {
    margin: 15px;
    font-size: 18px;
    text-align: center;
    color: #fff;
}
</style>
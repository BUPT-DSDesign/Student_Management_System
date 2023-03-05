<template>
  <div class="header">
    <div class="left">

    </div>
    <div class="right">
      <el-dropdown>
        <span class="el-dropdown-link">
          <img class="icon" src="../assets/image/boy.png" alt="">
        </span>
        <el-dropdown-menu>
          <el-dropdown-item>个人中心</el-dropdown-item>
          <el-dropdown-item @click.native="layout">退出</el-dropdown-item>
        </el-dropdown-menu>
      </el-dropdown>
    </div>
  </div>
</template>
<script>
export default {
  data() {
    return {}
  },
  methods: {
    layout() {
      console.log('输出')
      window.localStorage.removeItem('token') //移除token
      this.$router.replace('/')
    }
  },
}
</script>
<style>
.header {
  background-color: #343a40;
  height: 54px;
}

.el-icon-arrow-down {
  font-size: 12px;
}

.right {
  float: right;
  margin: 10px;
}

.icon {
  width: 35px;
  height: 35px;
  margin-right: 20px;
}
</style>
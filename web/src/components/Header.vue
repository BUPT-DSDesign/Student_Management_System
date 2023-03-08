<template>
  <div class="header">
    <div class="left">

    </div>
    <div class="right">
      <el-dropdown>
        <span class="el-dropdown-link">
          <img class="icon" :src="avatarUrl" alt="">
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
    return {
      avatarUrl: 'http://127.0.0.1:8080/static/avatar.jpg' // 默认头像
    }
  },
  mounted() {
     if (window.localStorage.getItem('avatar_url') != null) {
      this.avatarUrl = window.localStorage.getItem('avatar_url')
    }
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
  border-radius: 50%;
}
</style>
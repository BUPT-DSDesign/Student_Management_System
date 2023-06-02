<template>
  <div class="header">
    <div class="left">
     <mytime></mytime>
    </div>
    <div class="right">
      <el-dropdown>
        <span class="el-dropdown-link">
          <img class="icon" :src="avatarUrl" @error="changeToDefault">
        </span>
        <el-dropdown-menu>
          <el-dropdown-item @click.native="layout">退出</el-dropdown-item>
        </el-dropdown-menu>
      </el-dropdown>
    </div>
  </div>
</template>
<script>
import mytime from '@/components/mytime.vue'
import { TimeStore } from '@/store/time'
export default {
  data() {
    return {
      avatarUrl: '',// 默认头像
    }
  },
  mounted() {
    const userId = window.localStorage.getItem('userId')
    // console.log(userId)
    this.avatarUrl = `http://127.0.0.1:8080/static/${userId}.jpg`
    // this.avatarUrl = `http://127.0.0.1:8080/static/avatar.jpg`
  },
  computed: {
  
  },
  methods: {
    changeToDefault() {
      this.avatarUrl = `http://127.0.0.1:8080/static/avatar.jpg`
    },
    layout() {
      TimeStore.clear()
      window.localStorage.clear()
      this.$router.replace('/')
      location.reload()

    },
  },
  components: {
    mytime,
  }
}
</script>
<style>
@font-face {
	font-family: mFont;
	src: url(../assets/font/LcdD.ttf);
}

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
.weektitle{
  margin-left:20px;
  margin-top:10px;
  color: #eee;
  float: left;
  font-family: mFont;
}
.icon {
  width: 35px;
  height: 35px;
  margin-right: 20px;
  border-radius: 50%;
}
</style>
<template>
    <div class="wrapper">
        <div class="one item">
            <uploadAvatar :userInfo="userInfo"></uploadAvatar>
            <div class="user">
                <h1>{{ userInfo.username }}</h1>
            </div>
            <signature></signature>
        </div>
        <div class="two item">
            <div class="classblock" style="height: 120px;float: left;">
                <img src="../assets/image/class.png" style="height: 120px;width: 120px; margin-left:11px">
                <span class="right-class">今天还有<h6 style="font-size:25px"> {{ classNumber_remaining }}</h6>节课</span>
            </div>
            <div class="eventblock" style="height: 120px;float: left;">
                <img src="../assets/image/event.png" style="height: 120px;width: 120px; margin-left:11px">
                <span class="right-event">今天还有<h6 style="font-size:25px">{{ eventNumber_remaining }}</h6>个活动</span>
            </div>
        </div>
        <div class="three item">
            <div class="ProgressBar">
                <h6>本日进度</h6>
                <el-progress :percentage="60" :stroke-width="10"></el-progress>
                <h6>本周进度</h6>
                <el-progress :percentage="80" :stroke-width="10"></el-progress>
                <h6>本学期进度</h6>
                <el-progress :percentage="20" :stroke-width="10"></el-progress>
            </div>
        </div>
        <div class="four item">
            <todolist></todolist>
        </div>
    </div>
</template>
<script>
import uploadAvatar from '@/components/Homepage/uploadAvatar.vue';
import signature from '@/components/Homepage/signature.vue';
import todolist from '@/components/Homepage/todolist.vue'
import { useUserStore } from '@/pinia/modules/user';

export default {
    components: {
        uploadAvatar,
        signature,
        todolist,
    },
    beforeMount() {
        // 在个人主页渲染的时候, 应该向后端请求个人信息
        // console.log('这是父亲')
        console.log('发送请求')
        const getUserInfo = async () => {
            const fg = await this.useUserStore.GetUserInfo()
            if (fg) {
                console.log('获取成功')
                this.userInfo = this.useUserStore.userInfo
                // console.log(this.userInfo)
            } else {
                console.log('获取失败')
            }
        }
        getUserInfo()
    },
    data() {
        return {
            classNumber_remaining: 3,
            eventNumber_remaining: 2,
            useUserStore: new useUserStore(),
            userInfo: {}
        }
    },
}
</script>

<style>
.wrapper {
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    grid-gap: 20px;
    grid-auto-rows: minmax(150px, auto);
}

.one {
    grid-column: 1 / 2;
    grid-row: 1 / 3;
    background: #f8f9fa;

}

.two {
    grid-column: 2 / 4;
    grid-row: 1 / 2;
    background: #f8f9fa;
}

.three {
    grid-column: 1 / 2;
    grid-row: 3 / 5;
    background: #f8f9fa;
}

.four {
    grid-column: 2 / 4;
    grid-row: 2 / 5;
    background: #f8f9fa;
}

.item {
    text-align: center;
    font-size: 200%;
    color: #fff;
}

.user {
    margin-top: 50px;
    margin-right: 50px;
    float: right;
}

html {
    background-color: #e9ecef;
}



h6 {
    font-size: 15px;
}

.ProgressBar {
    margin-top: 30px;
}

.right-event,
.right-class {
    color: black;
    font-size: 16px;
    float: right;
    margin-top: 40px;
    margin-right: 40px;
    margin-left: 20px;
}
.el-progress{
    margin-top:15px;
}

.classblock,
.eventblock {
    background-color: #e9ecef;
    margin: 10px 47px;
}
</style>


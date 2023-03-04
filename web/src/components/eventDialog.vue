<template>
    <el-dialog title="课程详情" :visible.sync="dialogDetailVisible" width="30%" :before-close="handleClose">
        <el-card class="box-card">
            <div>活动名称：{{ selected.event }}</div>
            <div>活动周次：{{ selected.week }}</div>
            <div>活动地点：{{ selected.address }}</div>
            <div>活动类型：{{ selected.tag }}</div>
            <div>活动开始时间：{{ selected.time }}</div>
        </el-card>
        <span slot="footer" class="dialog-footer">
            <el-button @click="dialogDetailVisible = false">取 消</el-button>
            <el-button type="primary" @click="dialogDetailVisible = false">确 定</el-button>
        </span>
    </el-dialog>
</template>
<script>
   export default {
    data() {
           return {
        }
    },
    props: ['selected','dialogDetailVisible'],//接收父组件传过来的信息
    method: {
         // 关闭弹窗
        handleClose(done) {
            done();
        },
    }
}
</script>
<template>
    <el-dialog title="活动详情" :visible.sync="dialogDetailVisible" width="30%" >
            <div>活动名称：{{ selected.activity_name }}</div>
            <div>活动周次:{{ selected.date }}周</div>
            <div>活动地点：{{ selected.location }}</div>
            <div>活动类型：{{ selected.tag==0?'集体活动':'个人活动'}}</div>
            <div>活动开始时间：{{ selected.start_time }}</div>
            <div>活动闹钟(提前分钟数)：{{ selected.advance_mention_time }}</div>
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
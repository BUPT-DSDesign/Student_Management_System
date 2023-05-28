import { Register, Login, GetInfo, EditSignature, GetAllInfo } from '@/api/user'
import { defineStore } from 'pinia'
import { ref } from 'vue'
import { Loading } from 'element-ui'

export const useUserStore = defineStore('user', () => {
    const loadingInstance = ref(null) // 加载框

    // 注册
    const RegisterIn = async (registerInfo) => {
        loadingInstance.value = Loading.service({
            lock: true,
            text: '注册中，请稍候.....',
            background: 'rgba(0, 0, 0, 0.7)'
        })
        try {
            const res = await Register(registerInfo)
            if (res.data.status_code == 0) {
                initTokenAndId()
                // 注册成功后把token和userId存起来
                window.localStorage.setItem('token', res.data.token)
                window.localStorage.setItem('userId', res.data.user_id)
                let role = res.data.is_admin == true ? "admin" : "student";
                localStorage.setItem("role", role);

                loadingInstance.value.close()
                // ....其他操作
                return true
            }
            return false
        } catch (err) {
            loadingInstance.value.close()
            return false
        }
    }

    // 登录
    const initTokenAndId = () => {
        window.localStorage.removeItem('token') //移除token
        window.localStorage.removeItem('userId') //移除id
        window.localStorage.removeItem('role')
    }
    const LoginIn = async (loginInfo) => {
        loadingInstance.value = Loading.service({
            lock: true,
            text: '登录中，请稍候.....',
            background: 'rgba(0, 0, 0, 0.7)'
        })
        try {
            const res = await Login(loginInfo)
            console.log(res.data)
            if (res.data.status_code == 0) {
                initTokenAndId()
                // 登录成功后把token和userId存起来
                window.localStorage.setItem('token', res.data.token)
                window.localStorage.setItem('userId', res.data.user_id)
                console.log(res.data.is_admin)
                let role = res.data.is_admin == true ? "admin" : "student";
                localStorage.setItem("role", role);
                loadingInstance.value.close()
                // ....其他操作
                return true
            }

            return false
        } catch (err) {
            loadingInstance.value.close()
            return false
        }
    }

    // 获取用户信息
    const userInfo = ref({})
    const GetUserInfo = async () => {
        try {
            const res = await GetInfo()
            if (res.data.status_code == 0) {
                userInfo.value = res.data.user_info
                return true
            }
        } catch (err) {
            return false
        }
    }
    //获取所有学生信息
    const allStuInfo = ref({});
    const GetAllStuInfo = async () => {
        try {
            const res = await GetAllInfo()
            if (res.data.status_code == 0) {
                allStuInfo.value = res.data.user_infos
                return true
            }
        } catch (err) {
            return false
        }
    }

    // 编辑个性签名
    const EditUserSignature = async (signature) => {
        try {
            const res = await EditSignature(signature)

            if (res.data.status_code == 0) {
                console.log(res.data)
                return true
            }
        } catch (err) {
            return false
        }
    }

    return {
        RegisterIn,
        LoginIn,
        userInfo,
        allStuInfo,
        GetUserInfo,
        GetAllStuInfo,
        EditUserSignature
    }
})
